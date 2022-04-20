
import { DatePipe } from '@angular/common';
import { Item } from 'src/lib/data-access/models';
import { AfterViewInit, Component, ElementRef, EventEmitter, Input, Output, ViewChild } from "@angular/core";
import { BackendService } from 'src/lib/data-access/service/backend.service';


@Component({
    selector: 'item',
    templateUrl: './item.component.html',
    styleUrls: ['./item.component.scss'],
})
export class ItemComponent implements AfterViewInit {
    @Input() item: Item;
    @Input() selectedOnCreate: boolean;
    @Output() hoverEvent = new EventEmitter<number>();
    @Output() deleteEvent = new EventEmitter<number>();
    @Output() flagEvent = new EventEmitter<number>();
    @Output() dateChangedEvent = new EventEmitter<number>();
    @Output() updateTodayCounterEvent = new EventEmitter<number>();

    @ViewChild('myInput') myInput: ElementRef;

    elementRef: ElementRef;
    datePipe: DatePipe = new DatePipe('de');
    checked: boolean = false;

    constructor( private backendService: BackendService) { }

    ngAfterViewInit(): void {
        if (this.selectedOnCreate) {
            this.myInput.nativeElement.focus();
        }
    }

    getImgSource(): string {
        return this.item.flagged ? "assets/flag/flag-solid.svg" : "assets/flag/flag-regular.svg";
    }

    getDateOfToday(): string {
        return new Date().toISOString().split("T")[0];
    }

    onFlagChange(): void {
        this.item.flagged = !(this.item.flagged);
        this.flagEvent.emit(this.item.id);
        this.backendService.putItem(this.item).subscribe();
    }

    onTextChange(event: any): void {
        this.item.text = event.target.value;
        this.backendService.putItem(this.item).subscribe();
    }

    onDateChange(event: any): void {
        if (this.isToday(this.item.date) && !this.isToday(event.target.value)) {
            this.dateChangedEvent.emit(this.item.id);
            this.updateTodayCounterEvent.emit(-1);
        }
        this.item.date = event.target.value;
        if (this.isToday(this.item.date)) {
            this.updateTodayCounterEvent.emit(1);
        }

        this.backendService.putItem(this.item).subscribe();
    }

    onCheckboxEvent(): void {
        if (this.checked == false) {
            this.checked = true;
            this.onTaskCheck();
        } else {
            this.checked = false;
        }
    }

    onTaskCheck() {
        setTimeout(() => {
            if (this.checked == true) {
                this.deleteEvent.emit(this.item.id);
            }
        }, 3000);
    }

  isToday (someDate: string): boolean {
    const today: string = new Date().toISOString().substring(0, 10);
    return today == someDate;
  }
}
