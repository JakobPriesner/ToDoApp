import { Component, EventEmitter, Input, Output } from '@angular/core';


@Component({
    selector: 'filterButton',
    templateUrl: './filterButton.component.html',
    styleUrls: ['./filterButton.component.scss'],
})
export class FilterButtonComponent {
    @Input() value: string;
    @Input() count: number;
    @Input() iconPath: string;
    @Output() newClickEvent = new EventEmitter<any>();

    clickEvent(): void {
        this.newClickEvent.emit();
    }

}
